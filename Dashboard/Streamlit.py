import streamlit as st
import pandas as pd
import plotly.express as px

# Configuração da página
st.set_page_config(page_title="Relatório Técnico: Análise Empírica", layout="wide")

# Função para carregar os dados
@st.cache_data
def load_data(file_name):
    return pd.read_csv(file_name)

# --- INTERFACE DE ESCOLHA DA BASE ---
st.sidebar.title("Configurações do Relatório")
tipo_analise = st.sidebar.radio("Tipo de Algoritmo:", ["Ordenação", "Busca"])

if tipo_analise == "Ordenação":
    df = load_data('metricas_completa.csv')
else:
    df = load_data('metricas_busca_completa.csv')

# --- FILTROS ---
lista_dist = df['Distribuicao'].unique().tolist()
dist_selecionada = st.sidebar.selectbox("Condição (Distribuição/Alvo)", lista_dist)

lista_tam = sorted(df['TamanhoVetor'].unique().tolist())
tam_selecionado = st.sidebar.selectbox("Tamanho do Vetor (N)", lista_tam)

# Filtragem do dataframe
df_filtrado = df[
    (df['Distribuicao'] == dist_selecionada) & 
    (df['TamanhoVetor'] == tam_selecionado)
]

# Agrupamento para as Labels de Algoritmo
df_grouped = df_filtrado.groupby('Algoritmo').agg({
    'TempoNS': 'mean',
    'Comparacoes': 'mean',
    'Trocas': 'mean',
    'AcessosMemoria': 'mean',
    'MemoriaAuxiliarBytes': 'max'
}).reset_index()

# --- DASHBOARD ---

st.header(f"Análise de {tipo_analise}: {dist_selecionada} (N={tam_selecionado})")

# Linha 1: Tempo e Memória
col1, col2 = st.columns(2)

with col1:
    st.subheader("⏱️ Tempo Médio (ns)")
    fig_tempo = px.bar(df_grouped.sort_values('TempoNS', ascending=False), 
                       x='Algoritmo', y='TempoNS', color='Algoritmo',
                       text_auto='.2s', title="Eficiência de Tempo (Maior para Menor)")
    st.plotly_chart(fig_tempo, use_container_width=True)

with col2:
    st.subheader("💾 Acessos à Memória")
    fig_mem = px.bar(df_grouped.sort_values('AcessosMemoria', ascending=False), 
                     x='Algoritmo', y='AcessosMemoria', color='Algoritmo', text_auto='.2s',
                     title="Uso do Barramento de Dados (Total de Operações)")
    st.plotly_chart(fig_mem, use_container_width=True)

# Linha 2: Esforço e Espaço
col3, col4 = st.columns(2)

with col3:
    st.subheader("🔄 Esforço: Trocas")
    # Gráfico de barras relacionando Comparações (X) e Trocas (Y)
    fig_esforco = px.bar(df_grouped.sort_values('Trocas', ascending=False), 
                         x='Algoritmo', y='Trocas', color='Algoritmo',text_auto='.2s',
                         title="Volume de Trocas")
    st.plotly_chart(fig_esforco, use_container_width=True)

with col4:
    st.subheader("📦 Memória Auxiliar (Bytes)")
    fig_aux = px.bar(df_grouped.sort_values('MemoriaAuxiliarBytes', ascending=False), 
                     x='Algoritmo', y='MemoriaAuxiliarBytes', color='Algoritmo',text_auto='.2s',
                     title="Custo de Memória Adicional")
    st.plotly_chart(fig_aux, use_container_width=True)

# Tabela de Dados
with st.expander("Ver Tabelas de Referência (Média das Iterações)"):
    st.table(df_grouped.sort_values(by='TempoNS'))